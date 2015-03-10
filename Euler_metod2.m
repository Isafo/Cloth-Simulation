% ----------------------------------------%
% 8 springs and 8 dampers ----------------%
% 4 streatch springs and 4 shear springs -%
% ----------------------------------------%

clear all
close all

%norm = sqrt(x^2+y^2+x^2)

% n = tidsstegs nr 
% j = block nr
% b = antal kolumner
% H = tissteg längd (sekunder)
% k = fjäderkonstant (St = stretch Sh = shear)
% m = blockets massa (kg)
% oaSt = orginal avstånd mellan blocken (fjädrarnas viloläge) (meter)
% c = dämpningskonstant (St = stretch Sh = shear) prosent
% slutTid = hur lång tid simmuleringen kör (sekunder)

bredd = 10;
hojd = 10;

b = bredd;
h = 0.008;
m = 0.001;
kSt = 1.5;
kSh = 0.2;
oaSt = 1;
oaSh = sqrt(2*oaSt^2)
cSt = 0.011;
cSh = 0.004;
g = [0;0;9.82];
slutTid = 10;

particle = placeParticles(bredd,hojd,oaSt)
velocity = zeros(3,bredd*hojd);

staticPoint1 = bredd*hojd;
staticPoint2 = bredd*(hojd-1)+1;

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
 graf = scatter3(x,y,z,36);
    

for tid = 0:h:slutTid
    particle_new = zeros(size(particle));

    for j = 1:bredd*hojd  
        
        %if we can have a streatch spring upwards
        if j > b
            kUpp = ((particle_old(:,j-b)-particle_old(:,j)).*((norm(particle_old(:,j-b)-particle_old(:,j))-oaSt)/norm(particle_old(:,j-b)-particle_old(:,j))));
            cUpp = velocity_old(:,j-b)-velocity_old(:,j);
        else
            kUpp = 0;
            cUpp = 0;
        end
        
        %if we can have a streatch spring to the right
        if mod(j,b) ~= 0
            kHoger = ((particle_old(:,j+1)-particle_old(:,j)).*((norm(particle_old(:,j+1)-particle_old(:,j))-oaSt)/norm(particle_old(:,j+1)-particle_old(:,j))));
            cHoger = velocity_old(:,j+1)-velocity_old(:,j);
        else
            kHoger = 0;
            cHoger = 0;
        end
        
        %if we can have a streatch spring downwards
        if j <= b*(hojd-1)
            kNed = ((particle_old(:,j+b)-particle_old(:,j)).*((norm(particle_old(:,j+b)-particle_old(:,j))-oaSt)/norm(particle_old(:,j+b)-particle_old(:,j))));
            cNed = velocity_old(:,j+b)-velocity_old(:,j);
        else
            kNed = 0;
            cNed = 0;
        end
        
        %if we can have a streatch spring to the left
        if mod(j,b) ~= 1
            kVanster = ((particle_old(:,j-1)-particle_old(:,j)).*((norm(particle_old(:,j-1)-particle_old(:,j))-oaSt)/norm(particle_old(:,j-1)-particle_old(:,j))));
            cVanster = velocity_old(:,j-1)-velocity_old(:,j);
        else
            kVanster = 0;
            cVanster = 0;
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
        
        if j == staticPoint1 || j == staticPoint2 
                particle_new(:,j) = particle(:,j);
                velocity(:,j) = velocity_old(:,j);
        else    
            %calculate the new velosity
            velocity(:,j) = velocity_old(:,j)+(h/m).*(-m.*g + kSt.*(kUpp+kVanster+kHoger+kNed)+kSh.*(kUppVanster+kUppHoger+kNedVanster+kNedHoger)+cSt.*(cUpp+cVanster+cHoger+cNed)+cSh.*(cUppVanster+cUppHoger+cNedVanster+cNedHoger));
            
            %calculate the new possition 
            particle_new(:,j) = particle(:,j)+h.*velocity(:,j);
        end
    end
    
    particle_old = particle;
    particle = particle_new;
    
    velocity_old = velocity;
    
    if mod(tid/h,20) == 0
        %uppdat draw funktion
        x = particle(1,:);
        y = particle(2,:);
        z= particle(3,:);
        scatter3(x,y,z,36)   

        xlabel('x') % x-axis label
        ylabel('y') % y-axis label
        zlabel('z') % z-axis label
        axis([-(bredd*oaSt+1) (bredd*oaSt+1) -(hojd*oaSt+1) (hojd*oaSt+1) -(hojd*oaSt+1) (hojd*oaSt+1)])

        drawnow  %makes the scatterplott visible
    end
end







