clear all
close all

%norm = sqrt(x^2+y^2+x^2)

% n = tidsstegs nr 
% j = block nr
% b = antal kolumner
% H = tissteg längd (sekunder)
% k = fjäderkonstant
% m = blockets massa (kg)
% oa = orginal avstånd mellan blocken (fjädrarnas viloläge) (meter)
% c = dämpningskonstant
% g = gravitation
% slutTid = hur lång tid simmuleringen kör (sekunder)

bredd = 10;
hojd = 10;
b = bredd;
h = 0.002;
m = 0.0004;
k = -5;
oa = 0.2;
c = -0.03;
g = [0;0;9.82];
slutTid = 5;

particle = placeParticles(bredd,hojd,oa)
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
 
xlabel('x') % x-axis label
ylabel('y') % y-axis label
zlabel('z') % z-axis label

axis([-1 5 -1 5 -10 10])

for tid = 0:h:slutTid
    particle_new = zeros(size(particle));

    for j = 1:bredd*hojd  
            
            % if we are on the top row of the fabric
            if j <= b
                kUpp = 0;
                cUpp = 0;
            else 
                kUpp = ((particle_old(:,j-b)-particle_old(:,j)).*((norm(particle_old(:,j-b)-particle_old(:,j))-oa)/norm(particle_old(:,j-b)-particle_old(:,j))));
                cUpp = velocity_old(:,j-b)-velocity_old(:,j);
            end    
            
            % if we are on the far left column of the fabric
            if mod(j,b) == 1
                kVanster = 0;
                cVanster = 0;
            else
                kVanster = ((particle_old(:,j-1)-particle_old(:,j)).*((norm(particle_old(:,j-1)-particle_old(:,j))-oa)/norm(particle_old(:,j-1)-particle_old(:,j))));
                cVanster = velocity_old(:,j-1)-velocity_old(:,j);
            end
            
            % if we are on the far right column of the fabric    
            if mod(j,b) == 0
                kHoger = 0;
                cHoger = 0;
            else
                kHoger = ((particle_old(:,j+1)-particle_old(:,j)).*((norm(particle_old(:,j+1)-particle_old(:,j))-oa)/norm(particle_old(:,j+1)-particle_old(:,j))));
                cHoger = velocity_old(:,j+1)-velocity_old(:,j);
            end
            
            % if we are on the bottom row of the fabric  
            if j > b*(hojd-1)
                kNed = 0;
                cNed = 0;
            else
                kNed = ((particle_old(:,j+b)-particle_old(:,j)).*((norm(particle_old(:,j+b)-particle_old(:,j))-oa)/norm(particle_old(:,j+b)-particle_old(:,j))));
                cNed = velocity_old(:,j+b)-velocity_old(:,j);
            end
            
            
            if j == staticPoint1 || j == staticPoint2 
                particle_new(:,j) = particle(:,j);
                velocity(:,j) = velocity_old(:,j);
            else
                %calculate the new velosity (minustecknet vid k och c försvinner då k är neggativ)
                velocity(:,j) = velocity_old(:,j)+(h/m).*(-m.*g-k.*(kUpp+kVanster+kHoger+kNed)-c.*(cUpp+cVanster+cHoger+cNed));

                %calculate the new possition 
                particle_new(:,j) = particle(:,j)+h.*velocity(:,j);                       
            end
    end
    
    particle_old = particle;
    particle = particle_new;
    
    velocity_old = velocity;
    
    if mod(tid/h,70) == 0  
        %uppdate draw funktion
        x = particle(1,:);
        y = particle(2,:);
        z= particle(3,:);
        scatter3(x,y,z,36) 

        xlabel('x') % x-axis label
        ylabel('y') % y-axis label
        zlabel('z') % z-axis label

        axis([-(bredd+1)*oa (bredd+1)*oa -(hojd+1)*oa (hojd+1)*oa -(hojd+1)*oa (hojd+1)*oa])
        drawnow  %makes the scatterplott visible
    end
end