% ftype: 'Test.txt', 'Train.txt', 'Validation.txt'

function [B,count] = readTrainValTestIndices(ftype,basedir)

fname = [basedir ftype];

fid = fopen(fname,'r');
[A,count] = textscan(fid,'%s');
fclose(fid);

B = A{:};
    
