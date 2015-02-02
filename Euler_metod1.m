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
particle9 = [1.1; 50; 1];

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
% k = fjäderkonstant
% m = blockets massa (kg)
% oa = orginal avstånd mellan blocken (fjädrarnas viloläge) (meter)
% c = dämpningskonstant
% slutTid = hur lång tid simmuleringen kör (sekunder)

b = bredd;
h = 0.1;
m = 0.01;
k = -100;
oa = 1;
c = 0;
slutTid = 0.5;


velocity_old = velocity;
particle_old = particle;

%draw inisial state
figure
x = [particle(1,:)];
y = [particle(2,:)];
z = [particle(3,:)];
color = reshape([1,0,0,
                 0,1,0,
                 0,0,1,
                 1,1,0,
                 1,0,1,
                 0,1,1,
                 0,0,0,
                 0.5,0.5,0.5,
                 0,0.5,0.5],9,3);
scatter3(x,y,z,36,color)

for tid = 0:h:slutTid
    particle_new = zeros(size(particle));
    velocity_new = zeros(size(particle));

    for j = 1:bredd*hojd  
            
            % if we are on the top row of the fabric
            if j <= b
                kUpp = 0;
                cUpp = 0;
            else 
                kUpp = ((particle_old(:,j-b)-particle_old(:,j)).*(abs(norm(particle_old(:,j-b)-particle_old(:,j))-oa)/norm(particle_old(:,j-b)-particle_old(:,j))));
                cUpp = velocity_old(:,j-b)-velocity_old(:,j);
            end    
            
            % if we are on the far left column of the fabric
            if mod(j,b) == 1
                kVanster = 0;
                cVanster = 0;
            else
                kVanster = ((particle_old(:,j-1)-particle_old(:,j)).*(abs(norm(particle_old(:,j-1)-particle_old(:,j))-oa)/norm(particle_old(:,j-1)-particle_old(:,j))));
                cVanster = velocity_old(:,j-1)-velocity_old(:,j);
            end
            
            % if we are on the far right column of the fabric    
            if mod(j,b) == 0
                kHoger = 0;
                cHoger = 0;
            else
                kHoger = ((particle_old(:,j+1)-particle_old(:,j)).*(abs(norm(particle_old(:,j+1)-particle_old(:,j))-oa)/norm(particle_old(:,j+1)-particle_old(:,j))));
                cHoger = velocity_old(:,j+1)-velocity_old(:,j);
            end
            
            % if we are on the bottom row of the fabric  
            if j > b*(hojd-1)
                kNed = 0;
                cNed = 0;
            else
                kNed = ((particle_old(:,j+b)-particle_old(:,j)).*(abs(norm(particle_old(:,j+b)-particle_old(:,j))-oa)/norm(particle_old(:,j+b)-particle_old(:,j))));
                cNed = velocity_old(:,j+b)-velocity_old(:,j);
            end
            
            %calculate the new velosity
            velocity(:,j) = velocity_old(:,j)+(h/m).*(-k.*(kUpp+kVanster+kHoger+kNed)-c.*(cUpp+cVanster+cHoger+cNed));
            
            %calculate the new possition 
            particle_new(:,j) = particle(:,j)+h.*velocity(:,j);
    end
    
    particle_old = particle;
    particle = particle_new;
    
    velocity_old = velocity;
    velocity = velocity_new;
    
    %uppdat draw funktion
    x = [particle(1,:)];
    y = [particle(2,:)];
    z = [particle(3,:)];
    %drawnow update 
   % hold on
    %scatter3(x,y,z)
    figure
    color = reshape([1,0,0,
                     0,1,0,
                     0,0,1,
                     1,1,0,
                     1,0,1,
                     0,1,1,
                     1,1,1
                     0.5,0.5,0.5,
                     0,0.5,0.5],9,3);
    scatter3(x,y,z,36,color)
end


%%
clear all

x = linspace(0,10,10000);
y = sin(x);
%plot(x,y)
hold on
p = plot(0,0,'o','MarkerFaceColor','red');
hold off
axis manual

for k = 2:length(x)
    p.XData = x(k);
    p.YData = y(k);
    drawnow update
end

%%
clear all

figure
Z = peaks;
surf(Z)
axis tight manual
ax = gca;
ax.NextPlot = 'replaceChildren';


loops = 40;
F(loops) = struct('cdata',[],'colormap',[]);
for j = 1:loops
    X = sin(j*pi/10)*Z;
    surf(X,Z)
    drawnow
    F(j) = getframe;
end

%%
clear all

load seamount

figure
hs(1) = subplot(2,1,1);
hs(2) = subplot(2,1,2);
scatter3(hs(1),x,y,z,'MarkerFaceColor',[0 .75 .75])
scatter3(hs(2),x,y,z,'*')

figure
[X,Y,Z] = sphere(16);
S = repmat([50,25,10],numel(X),1);
C = repmat([1,2,3],numel(X),1);
s = S(:);
c = C(:);
x = [0.5*X(:)];
y = [0.5*Y(:)];
z = [0.5*Z(:)];
scatter3(x,y,z)
