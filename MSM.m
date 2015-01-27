function [grid] = MSM(width, height)
    grid = zeros(width, height, 3);
    springRestLength = 2;
    
    %Place the particles in a coordinate system the first particle in origo
    %first channel contains the x, secound channel y and 3
    %channel z coordinate
    for i = 1:height
        for j = 1:width
            grid(i, j, 1) = springRestLength * j - springRestLength; %x-coordinate
            grid(i, j, 2) = springRestLength * i - springRestLength; %y-coordinate
            grid(i, j, 3) = 0; %z-coordinate
        end
    end
end