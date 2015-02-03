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

%velocity = zeros(3,3,3);
velocity = 0;%[velocity1, velocity2, velocity3, velocity4, velocity5, velocity6, velocity7, velocity8, velocity9];
vmark = 0;
%particle = zeros(3,3,3);
particle = [0; 3; 0]; %[particle1, particle2, particle3, particle4, particle5, particle6, particle7, particle8, particle9];
mark = [0; 0; 0];

% n = tidsstegs nr 
% j = block nr
% b = antal kolumner
% H = tissteg längd (sekunder) 
% k = fjäderkonstant
% m = blockets massa (kg)
% oa = orginal avstånd mellan blocken (fjädrarnas viloläge) (meter)
% c = dämpningskonstant
% slutTid = hur lång tid simmuleringen kör (sekunder)

%b = bredd;
h = 0.1;
m = 0.5;
k = 10;
oa = 2;
c = 5;
slutTid = 1;


velocity_old = velocity;
particle_old = particle;

%x1 = x0 + h*v0
particle_new = particle + h*velocity
    
velocity_old = velocity;
particle_old = particle;

%starts at x2
for tid = h:h:slutTid
    particle_new = zeros(size(particle));
    velocity_new = zeros(size(particle));

    langd = norm(mark-particle_old);  
    kF = ((mark - particle_old).*(langd-oa/langd));
    cF = vmark - velocity_old;                    
            
            
            %calculate the new velosity
            velocity = velocity_old+(h/m).*(k.*(kF)+c.*(cF));
            
            %calculate the new possition 
            particle_new = particle + h.*velocity
    
    
    particle_old = particle;
    particle = particle_new;
    
    velocity_old = velocity;
    velocity = velocity_new;
    
    %uppdat draw funktion
    x = [particle(1)];
    y = [particle(2)];
    z = [particle(3)];
    %drawnow update 
   % hold on
    %scatter3(x,y,z)
    figure
    color = [1 0 0];
    scatter3(x,y,z)
end







