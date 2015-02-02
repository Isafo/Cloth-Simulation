% ----------------------------------------%
% 8 springs and 8 dampers ----------------%
% 4 streatch springs and 4 shear springs -%
% ----------------------------------------%

clear all

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
h = 0.1;
m = 0.01;
kSt = 100;
kSh = 100;
oa = 1;
cSt = 5;
cSh = 5;
slutTid = 5;


velocity_old = velocity;
particle_old = particle;
    
for tid = 0:h:slutTid
    particle_new = zeros(size(particle));
    velocity_new = zeros(size(particle));

    for j = 1:bredd*hojd  
            
            % if we aren't on the top row or the far left column of the fabric
            if j > b && mod(j,b) ~= 1
                kUppVanster = ((particle_old(:,j-b-1)-particle_old(:,j)).*(abs(norm(particle_old(:,j-b-1)-particle_old(:,j))-oa)/norm(particle_old(:,j-b-1)-particle_old(:,j))));
                cUppVanster = velocity_old(:,j-b-1)-velocity_old(:,j);
                kUpp = ((particle_old(:,j-b)-particle_old(:,j)).*(abs(norm(particle_old(:,j-b)-particle_old(:,j))-oa)/norm(particle_old(:,j-b)-particle_old(:,j))));
                cUpp = velocity_old(:,j-b)-velocity_old(:,j);
                kVanster = ((particle_old(:,j-1)-particle_old(:,j)).*(abs(norm(particle_old(:,j-1)-particle_old(:,j))-oa)/norm(particle_old(:,j-1)-particle_old(:,j))));
                cVanster = velocity_old(:,j-1)-velocity_old(:,j);
            else          
                kUppVanster = 0;
                cUppVanster = 0;
                kUpp = 0;
                cUpp = 0;
                kVanster = 0;
                cVanster = 0;
            end
            
            % if we aren't on the top row or the far right column of the fabric
            if j > b && mod(j,b) ~= 0
                kUpp = ((particle_old(:,j-b)-particle_old(:,j)).*(abs(norm(particle_old(:,j-b)-particle_old(:,j))-oa)/norm(particle_old(:,j-b)-particle_old(:,j))));
                cUpp = velocity_old(:,j-b)-velocity_old(:,j);
                kUppHoger = ((particle_old(:,j-b+1)-particle_old(:,j)).*(abs(norm(particle_old(:,j-b+1)-particle_old(:,j))-oa)/norm(particle_old(:,j-b+1)-particle_old(:,j))));
                cUppHoger = velocity_old(:,j-b+1)-velocity_old(:,j);
                kHoger = ((particle_old(:,j+1)-particle_old(:,j)).*(abs(norm(particle_old(:,j+1)-particle_old(:,j))-oa)/norm(particle_old(:,j+1)-particle_old(:,j))));
                cHoger = velocity_old(:,j+1)-velocity_old(:,j);
            else
                kUpp = 0;
                cUpp = 0;
                kUppHoger = 0;
                cUppHoger = 0;
                kHoger = 0;
                cHoger = 0;
            end
            
            % if we aren't on the bottom row or the far right column of the fabric
            if j <= b*(hojd-1) && mod(j,b) ~= 0
                kHoger = ((particle_old(:,j+1)-particle_old(:,j)).*(abs(norm(particle_old(:,j+1)-particle_old(:,j))-oa)/norm(particle_old(:,j+1)-particle_old(:,j))));
                cHoger = velocity_old(:,j+1)-velocity_old(:,j);
                kNedHoger = ((particle_old(:,j+b+1)-particle_old(:,j)).*(abs(norm(particle_old(:,j+b+1)-particle_old(:,j))-oa)/norm(particle_old(:,j+b+1)-particle_old(:,j))));
                cNedHoger = velocity_old(:,j+b+1)-velocity_old(:,j);
                kNed = ((particle_old(:,j+b)-particle_old(:,j)).*(abs(norm(particle_old(:,j+b)-particle_old(:,j))-oa)/norm(particle_old(:,j+b)-particle_old(:,j))));
                cNed = velocity_old(:,j+b)-velocity_old(:,j);
            else
                kHoger = 0;
                cHoger = 0;
                kNedHoger = 0;
                cNedHoger = 0;
                kNed = 0;
                cNed = 0;
            end
            
            % if we aren't on the bottom row or the far left column of the fabric
            if j <= b*(hojd-1) && mod(j,b) ~= 1
                kNed = ((particle_old(:,j+b)-particle_old(:,j)).*(abs(norm(particle_old(:,j+b)-particle_old(:,j))-oa)/norm(particle_old(:,j+b)-particle_old(:,j))));
                cNed = velocity_old(:,j+b)-velocity_old(:,j);
                kNedVanster = ((particle_old(:,j+b-1)-particle_old(:,j)).*(abs(norm(particle_old(:,j+b-1)-particle_old(:,j))-oa)/norm(particle_old(:,j+b-1)-particle_old(:,j))));
                cNedVanster = velocity_old(:,j+b-1)-velocity_old(:,j);
                kVanster = ((particle_old(:,j-1)-particle_old(:,j)).*(abs(norm(particle_old(:,j-1)-particle_old(:,j))-oa)/norm(particle_old(:,j-1)-particle_old(:,j))));
                cVanster = velocity_old(:,j-1)-velocity_old(:,j);
            else
                kNed = 0;
                cNed = 0;
                kNedVanster = 0;
                cNedVanster = 0;
                kVanster = 0;
                cVanster = 0;
            end
            
            %calculate the new velosity
            velocity(:,j) = velocity_old(:,j)+(h/m).*(-kSt.*(kUpp+kVanster+kHoger+kNed)-kSh.*(kUppVanster+kUppHoger+kNedVanster+kNedHoger)-cSt.*(cUpp+cVanster+cHoger+cNed)-cSh.*(cUppVanster+cUppHoger+cNedVanster+cNedHoger));
            
            %calculate the new possition 
            particle_new(:,j) = particle(:,j)+h.*velocity(:,j);
    end
    
    particle_old = particle;
    particle = particle_new;
    
    velocity_old = velocity;
    velocity = velocity_new;
end







