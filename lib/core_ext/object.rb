require 'v8'

class Object
  # From now on each object can be easily converted to <tt>V8::*</tt> object
  # using <tt>#to_v8</tt> method. 
  include V8::Cast

  # Returns <tt>true</tt> when given object is reflected from within V8.
  def v8?
    kind_of?(V8::Data)
  end
end # Object
