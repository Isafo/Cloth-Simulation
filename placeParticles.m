function [grid] = placeParticles(width, height)
    grid = zeros(3, width*height);
    springRestLength = 2;
    row = 0;
    
    %Place the particles in a coordinate system the first particle in origo
    %first row contains all particles x coordinates 2nd row y and 3rd z coordinates. - spring rest
    %lenght at the end because matlab cannot begin from 0
    for i = 1:height*width
        grid(1, i) = springRestLength * i - springRestLength; %x-coordinate
        if(i/width == 1) %check if on last particle in row
            grid(2, i) = row; %y-coordinate
            row =+ springRestLength; %increment row  
        else
            grid(2, i) = row; %y-coordinate
        end
    end
end