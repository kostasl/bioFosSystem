%% Bioluminesence Experiments data Handling
%% 

%% import Files 
%Put the script dir in the path
addpath(fileparts(which('processFilesTracks.m')))
 % fileparts(filename) is missing
 ss = '';
 files = dir(strcat(fileparts(which('importBiolumDat.m')),'/../dat/mb247xga/*.txt'));
 cd (strcat(fileparts(which('importBiolumDat.m')),'/../dat/mb247xga/'));
 
 biolum_mb247 = cell(numel(files),1)
for ff = 1:numel(files)
   %sfilename = sprintf('%s', ff.name);
   %display(sfilename);
  if (files(ff).isdir == 1)
      continue;
  end    
  files(ff).name
  biolum_mb247{ff,1} = importdata(files(ff).name);
  biolum_mb247{ff,2} = files(ff).name;
  
end

%No import 201YgatM6b
files = dir(strcat(fileparts(which('importBiolumDat.m')),'/../dat/201YgatM6b/*.txt'));
 cd (strcat(fileparts(which('importBiolumDat.m')),'/../dat/201YgatM6b/'));
 
 biolum_201YgatM6b = cell(numel(files),1)
for ff = 1:numel(files)
   %sfilename = sprintf('%s', ff.name);
   %display(sfilename);
  if (files(ff).isdir == 1)
      continue;
  end    
  files(ff).name
  biolum_201YgatM6b{ff,1} = importdata(files(ff).name);
  biolum_201YgatM6b{ff,2} = files(ff).name;
end

clear ff;
cd (strcat(fileparts(which('importBiolumDat.m')),'/../dat/'));

save('biolum.mat')