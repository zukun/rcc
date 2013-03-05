mkdir('/projects/vision/9/jaechul/caltech101/Result/');
dirinfo = dir('/projects/vision/9/jaechul/caltech101/101_ObjectCategories');
dirinfo = dirinfo(3:end); % 1,2 : root dir

for i = 1 : 101
    mkdir(['/projects/vision/9/jaechul/caltech101/Result/' dirinfo(i).name]);
end

    