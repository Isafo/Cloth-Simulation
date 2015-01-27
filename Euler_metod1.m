clear all

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

velocity = zeros(3,3,3);
velocity = [velocity1, velocity2, velocity3; velocity4, velocity5, velocity6; velocity7, velocity8, velocity9];
particle = zeros(3,3,3);
particle = [particle1, particle2, particle3; particle4, particle5, particle6; particle7, particle8, particle9];
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


    velocity_gammal = velocity;
    particle_gammal = particle;
    particle_ny = zeros(size(particle));

    for j = 1:bredd
        for i = 1:hojd
        
            %if i mitten
            %particle_ny(i,j) = particle(i,j) + h .* velocity(i,j) + (h^2 /m)*( -k *((particle(i-1,j)- particle(i,j)*(abs(abs(particle())
            
        end
    end








