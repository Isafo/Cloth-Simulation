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
% H = tissteg längd 
% k = fjäderkonstant
% m = blockets massa 
% oa = orginal avstånd mellan blocken (fjädrarnas viloläge)
% c = dämpningskonstant

b = bredd;
h = 0.1;
m = 0.01;
k = 100;
oa = 1;
c = 5;

    velocity_old = velocity;
    particle_old = particle;
    particle_new = zeros(size(particle));
    velocity_new = zeros(size(particle));

    for j = 5:5 %1:bredd*hojd
                        
            %if i mitten
            
            kUpp = ((particle_old(:,j-b)-particle_old(:,j)).*(abs(norm(particle_old(:,j-b)-particle_old(:,j))-oa)/norm(particle_old(:,j-b)-particle_old(:,j))));
            kVanster = ((particle_old(:,j-1)-particle_old(:,j)).*(abs(norm(particle_old(:,j-1)-particle_old(:,j))-oa)/norm(particle_old(:,j-1)-particle_old(:,j))));
            kHoger = ((particle_old(:,j+1)-particle_old(:,j)).*(abs(norm(particle_old(:,j+1)-particle_old(:,j))-oa)/norm(particle_old(:,j+1)-particle_old(:,j))));
            kNed = ((particle_old(:,j+b)-particle_old(:,j)).*(abs(norm(particle_old(:,j+b)-particle_old(:,j))-oa)/norm(particle_old(:,j+b)-particle_old(:,j))));
            
            cUpp = velocity_old(:,j-b)-velocity_old(:,j);
            cVanster = velocity_old(:,j-1)-velocity_old(:,j);
            cHoger = velocity_old(:,j+1)-velocity_old(:,j);
            cNed = velocity_old(:,j+b)-velocity_old(:,j);
            
            particle_new(:,j) = particle(:,j)+h.*velocity_old(:,j)+(h^2 /m).*(-k.*(kUpp+kVanster+kHoger+kNed)-c.*(cUpp+cVanster+cHoger+cNed));
    end








