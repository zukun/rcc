function assert_(condition,varargin)

if condition, return;
else
  error(varargin{:});
end
