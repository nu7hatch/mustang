class Class
  # Returns list of public instance methods of current class excluding methods
  # inherited from the <tt>Object</tt>.
  def declared_methods
    klass, result = self, []

    while not klass == Object
      result += klass.instance_methods(false)
      klass = klass.superclass
    end

    result
  end
end
