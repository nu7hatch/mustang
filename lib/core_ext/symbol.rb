class Symbol
  # Returns symbol with function name normalized for javascript. eg:
  #
  #   :foo.to_js_func_name     # => :foo
  #   :'foo='.to_js_func_name  # => :set_foo
  #   :'foo!'.to_js_func_name  # => :foo_bang
  #   :'foo?'.to_js_func_name  # => :is_foo
  #   :'+'.to_js_func_name     # => nil
  #
  def to_js_func_name
    js = self.to_s
    js = "set_#{js}" if js =~ /\=$/ 
    js = "#{js}_bang" if js =~ /\!$/ 
    js = "is_#{js}" if js =~ /\?$/ 
    js = js.gsub(/[^\w\d\_]+/, "")
    js.empty? ? nil : js.to_sym 
  end
end # Symbol
