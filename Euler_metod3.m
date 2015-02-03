% --------------------------------------------------------%
% 12 springs and 12 dampers ------------------------------%
% 4 streatch springs, 4 shear springs and 4 bend springs -%
% --------------------------------------------------------%

clear all
close all

% particle = zeros(3,3,3);
% particle(1,1,:) = [-1; 50; -1];
% particle(1,2,:) = [0; 50; -1];
% particle(1,3,:) = [1; 50; -1];
% particle(2,1,:) = [-1; 50; 0];
% particle(2,2,:) = [0; 50; 0];
% particle(2,3,:) = [1; 50; 0];
% particle(3,1,:) = [-1; 50; 1];
% particle(3,2,:) = [0; 50; 1];
% particle(3,3,:) = [1; 50; 1];
% 
% velocity1 = [0;0;0];
% velocity2 = [0;0;0];
% velocity3 = [0;0;0];
% velocity4 = [0;0;0];
% velocity5 = [0;0;0];
% velocity6 = [0;0;0];
% velocity7 = [0;0;0];
% velocity8 = [0;0;0];
% velocity9 = [0;0;0];
% 
% velocity = zeros(3,3,3);
% velocity(1,1,:) = [0;0;0];
% velocity(1,2,:) = [0;0;0];
% velocity(1,3,:) = [0;0;0];
% velocity(2,1,:) = [0;0;0];
% velocity(2,2,:) = [0;0;0];
% velocity(2,3,:) = [0;0;0];
% velocity(3,1,:) = [0;0;0];
% velocity(3,2,:) = [0;0;0];
% velocity(3,3,:) = [0;0;0];

% particle1 = [-1 50 -1];
% particle2 = [0 50 -1];
% particle3 = [1 50 -1];
% particle4 = [-1 50 0];
% particle5 = [0 50 0];
% particle6 = [1 50 0];
% particle7 = [-1 50 1];
% particle8 = [0 50 1];
% particle9 = [1 50 1];

particle1 = [-1; 50; -1];
particle2 = [0; 50; -1];
particle3 = [1; 50; -1];
particle4 = [-1; 50; 0];
particle5 = [0; 50; 0];
particle6 = [1; 50; 0];
particle7 = [-1; 50; 1];
particle8 = [0; 50; 1];
particle9 = [1; 50; 1];

velocity1 = [0;0;0];
velocity2 = [0;0;0];
velocity3 = [0;0;0];
velocity4 = [0;0;0];
velocity5 = [0;0;0];
velocity6 = [0;0;0];
velocity7 = [0;0;0];
velocity8 = [0;0;0];
velocity9 = [0;0;0];

%velocity = zeros(3,3,3);
velocity = [velocity1, velocity2, velocity3, velocity4, velocity5, velocity6, velocity7, velocity8, velocity9];
%particle = zeros(3,3,3);
particle = [particle1, particle2, particle3, particle4, particle5, particle6, particle7, particle8, particle9];

bredd = 3;
hojd = 3;
%norm = sqrt(x^2+y^2+x^2)

% n = tidsstegs nr 
% j = block nr
% b = antal kolumner
% H = tissteg längd (sekunder)
% k = fjäderkonstant (St = stretch Sh = shear)
% m = blockets massa (kg)
% oa = orginal avstånd mellan blocken (fjädrarnas viloläge) (meter)
% c = dämpningskonstant (St = stretch Sh = shear)
% slutTid = hur lång tid simmuleringen kör (sekunder)

b = bredd;
h = 0.001;
m = 0.01;
kSt = 1;
kSh = 1;
kB = 1;
oaSt = 1;
oaSh = sqrt(2*oaSt^2);
oaB = 2*oaSt;
cSt = 1;
cSh = 1;
cB = 1;
slutTid = 1;


velocity_old = velocity;
particle_old = particle;

%draw inisial state
x = particle(1,:);
y = particle(2,:);
z = particle(3,:);
color = reshape([1,0,0,
                 0,1,0,
                 0,0,1,
                 1,1,0,
                 1,0,1,
                 0,1,1,
                 0,0,0,
                 0.5,0.5,0.5,
                 0,0.5,0.5],9,3);
 graf = scatter3(x,y,z,36,color);
    
for tid = 0:h:slutTid
    particle_new = zeros(size(particle));
    velocity_new = zeros(size(particle));

    for j = 1:bredd*hojd  
        
        %if we can have a bend spring upwards
        if j > 2*b
            k2Upp = ((particle_old(:,j-2*b)-particle_old(:,j)).*((norm(particle_old(:,j-2*b)-particle_old(:,j))-oaB)/norm(particle_old(:,j-2*b)-particle_old(:,j))));
            c2Upp = velocity_old(:,j-2*b)-velocity_old(:,j);
            
            %then we can elso have a streatch spring upwards
            kUpp = ((particle_old(:,j-b)-particle_old(:,j)).*((norm(particle_old(:,j-b)-particle_old(:,j))-oaSt)/norm(particle_old(:,j-b)-particle_old(:,j))));
            cUpp = velocity_old(:,j-b)-velocity_old(:,j);
        else
            %if we can have a streatch spring upwards
            if j > b
                kUpp = ((particle_old(:,j-b)-particle_old(:,j)).*((norm(particle_old(:,j-b)-particle_old(:,j))-oaSt)/norm(particle_old(:,j-b)-particle_old(:,j))));
                cUpp = velocity_old(:,j-b)-velocity_old(:,j);
            else
                kUpp = 0;
                cUpp = 0;
            end
            k2Upp = 0;
            c2Upp = 0;
        end
        
        %if we can have a bend spring to the right
        if mod(j,b) > 0 && mod(j,b) < b-1
            k2Hoger = ((particle_old(:,j+2)-particle_old(:,j)).*((norm(particle_old(:,j+2)-particle_old(:,j))-oaB)/norm(particle_old(:,j+2)-particle_old(:,j))));
            c2Hoger = velocity_old(:,j+2)-velocity_old(:,j);
            
            %then we can elso have a streatch spring to the right
            kHoger = ((particle_old(:,j+1)-particle_old(:,j)).*((norm(particle_old(:,j+1)-particle_old(:,j))-oaSt)/norm(particle_old(:,j+1)-particle_old(:,j))));
            cHoger = velocity_old(:,j+1)-velocity_old(:,j);
        else
            %if we can have a streatch spring to the right
            if mod(j,b) ~= 0
                kHoger = ((particle_old(:,j+1)-particle_old(:,j)).*((norm(particle_old(:,j+1)-particle_old(:,j))-oaSt)/norm(particle_old(:,j+1)-particle_old(:,j))));
                cHoger = velocity_old(:,j+1)-velocity_old(:,j);
            else
                kHoger = 0;
                cHoger = 0;
            end
            k2Hoger = 0;
            c2Hoger = 0;
        end
        
        %if we can have a bend spring downwards
        if j <= b*(hojd-2)
            k2Ned = ((particle_old(:,j+2*b)-particle_old(:,j)).*((norm(particle_old(:,j+2*b)-particle_old(:,j))-oaB)/norm(particle_old(:,j+2*b)-particle_old(:,j))));
            c2Ned = velocity_old(:,j+2*b)-velocity_old(:,j);
            
            %then we can elso have a streatch spring downwards
            kNed = ((particle_old(:,j+b)-particle_old(:,j)).*((norm(particle_old(:,j+b)-particle_old(:,j))-oaSt)/norm(particle_old(:,j+b)-particle_old(:,j))));
            cNed = velocity_old(:,j+b)-velocity_old(:,j);
        else
            %if we can have a streatch spring downwards
            if j <= b*(hojd-1)
                kNed = ((particle_old(:,j+b)-particle_old(:,j)).*((norm(particle_old(:,j+b)-particle_old(:,j))-oaSt)/norm(particle_old(:,j+b)-particle_old(:,j))));
                cNed = velocity_old(:,j+b)-velocity_old(:,j);
            else
                kNed = 0;
                cNed = 0;
            end
            k2Ned = 0;
            c2Ned = 0;
        end
        
        %if we can have a bend spring to the left
        if mod(j,b) > 2
            k2Vanster = ((particle_old(:,j-2)-particle_old(:,j)).*((norm(particle_old(:,j-2)-particle_old(:,j))-oaB)/norm(particle_old(:,j-2)-particle_old(:,j))));
            c2Vanster = velocity_old(:,j-2)-velocity_old(:,j);
            
            %then we can elso have a streatch spring to the left
            kVanster = ((particle_old(:,j-1)-particle_old(:,j)).*((norm(particle_old(:,j-1)-particle_old(:,j))-oaSt)/norm(particle_old(:,j-1)-particle_old(:,j))));
            cVanster = velocity_old(:,j-1)-velocity_old(:,j);
        else
            %if we can have a streatch spring to the left
            if mod(j,b) ~= 1
                kVanster = ((particle_old(:,j-1)-particle_old(:,j)).*((norm(particle_old(:,j-1)-particle_old(:,j))-oaSt)/norm(particle_old(:,j-1)-particle_old(:,j))));
                cVanster = velocity_old(:,j-1)-velocity_old(:,j);
            else
                kVanster = 0;
                cVanster = 0;
            end
            k2Vanster = 0;
            c2Vanster = 0;
        end
        
        %if we can have shear spring to the right and uppwards
        if mod(j,b) > 0 && j > b
            kUppHoger = ((particle_old(:,j-b+1)-particle_old(:,j)).*((norm(particle_old(:,j-b+1)-particle_old(:,j))-oaSh)/norm(particle_old(:,j-b+1)-particle_old(:,j))));
            cUppHoger = velocity_old(:,j-b+1)-velocity_old(:,j);
        else
            kUppHoger = 0;
            cUppHoger = 0;
        end
        
        %if we can have shear spring to the right and downwards
        if mod(j,b) > 0 && j < b*(hojd-1)
            kNedHoger = ((particle_old(:,j+b+1)-particle_old(:,j)).*((norm(particle_old(:,j+b+1)-particle_old(:,j))-oaSh)/norm(particle_old(:,j+b+1)-particle_old(:,j))));
            cNedHoger = velocity_old(:,j+b+1)-velocity_old(:,j);
        else
            kNedHoger = 0;
            cNedHoger = 0;
        end
        
        %if we can have shear spring to the left and downwards
        if mod(j,b) ~= 1 && j < b*(hojd-1)
            kNedVanster = ((particle_old(:,j+b-1)-particle_old(:,j)).*((norm(particle_old(:,j+b-1)-particle_old(:,j))-oaSh)/norm(particle_old(:,j+b-1)-particle_old(:,j))));
            cNedVanster = velocity_old(:,j+b-1)-velocity_old(:,j);
        else
            kNedVanster = 0;
            cNedVanster = 0;
        end
        
        %if we can have shear spring to the left and uppwards
        if mod(j,b) ~= 1 && j > b
            kUppVanster = ((particle_old(:,j-b-1)-particle_old(:,j)).*((norm(particle_old(:,j-b-1)-particle_old(:,j))-oaSh)/norm(particle_old(:,j-b-1)-particle_old(:,j))));
            cUppVanster = velocity_old(:,j-b-1)-velocity_old(:,j);
        else
            kUppVanster = 0;
            cUppVanster = 0;
        end
        
            
            %calculate the new velosity
            velocity(:,j) = velocity_old(:,j)+(h/m).*(kSt.*(kUpp+kVanster+kHoger+kNed)+kSh.*(kUppVanster+kUppHoger+kNedVanster+kNedHoger)+kB.*(k2Upp+k2Hoger+k2Ned+k2Vanster)+cSt.*(cUpp+cVanster+cHoger+cNed)+cSh.*(cUppVanster+cUppHoger+cNedVanster+cNedHoger)+cB.*(c2Upp+c2Hoger+c2Ned+c2Vanster));
            
            %calculate the new possition 
            particle_new(:,j) = particle(:,j)+h.*velocity(:,j);
    end
    
    particle_old = particle;
    particle = particle_new;
    
    velocity_old = velocity;
    velocity = velocity_new;
    
  
    %uppdat draw funktion
    x = particle(1,:);
    y = particle(2,:);
    z= particle(3,:);
    scatter3(x,y,z,36,color)        
    drawnow  %makes the scatterplott visible
end







