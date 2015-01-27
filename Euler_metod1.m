clear all

particle = zeros(3,3,3);
particle(1,1,:) = [-1; 50; -1];
particle(1,2,:) = [0; 50; -1];
particle(1,3,:) = [1; 50; -1];
particle(2,1,:) = [-1; 50; 0];
particle(2,2,:) = [0; 50; 0];
particle(2,3,:) = [1; 50; 0];
particle(3,1,:) = [-1; 50; 1];
particle(3,2,:) = [0; 50; 1];
particle(3,3,:) = [1; 50; 1];

velocity1 = [0;0;0];
velocity2 = [0;0;0];
velocity3 = [0;0;0];
velocity4 = [0;0;0];
velocity5 = [0;0;0];
velocity6 = [0;0;0];
velocity7 = [0;0;0];
velocity8 = [0;0;0];
velocity9 = [0;0;0];

velocity = zeros(3,3,3);
velocity(1,1,:) = [0;0;0];
velocity(1,2,:) = [0;0;0];
velocity(1,3,:) = [0;0;0];
velocity(2,1,:) = [0;0;0];
velocity(2,2,:) = [0;0;0];
velocity(2,3,:) = [0;0;0];
velocity(3,1,:) = [0;0;0];
velocity(3,2,:) = [0;0;0];
velocity(3,3,:) = [0;0;0];


bredd = 3;
hojd = 3;


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

    velocity_old = velocity;
    particle_old = particle;
    particle_new = zeros(size(particle));
    velocity_new = zeros(size(particle));

    for j = 2:2
        for i = 2:2
                        
            %if i mitten
            
            upp = abs(abs(particle(i-1,j,:)-particle(i,j,:))-oa)/abs(particle(i-1,j,:)-particle(i,j,:))))
            particle_new(i,j,:) = particle(i,j,:) + h .* velocity_old(i,j,:) +  (h^2 /m).*(-k.*((particle(i-1,j,:)-particle(i,j,:)).*(abs(abs(particle(i-1,j,:)-particle(i,j,:))-oa)/abs(particle(i-1,j,:)-particle(i,j,:)))))
            %particle_new(i,j,:) = [1;2;3]
        end
    end








