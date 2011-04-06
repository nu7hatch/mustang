module Mustang
  module V8
    class Object
      # We have to extend native <tt>.new</tt> method with full support for reflection
      # of ruby objects. 
      class << self
        alias_method :native_new, :new

        def new(*args, &block)
          res = native_new(*args)
          res.send(:reflect_from, args.first)        
        end
      end
      
      def respond_to?(meth) # :nodoc:
        !self[meth].undefined? or super
      end

      def method_missing(meth, *args, &block) # :nodoc:
        if respond_to?(meth)
          property = self[meth]
          if property.is_a?(Function)
            return property.call_on(self, *args, &block)
          else
            return property
          end
        end
        super
      end

      include Comparable
      include Enumerable
      include Delegated

      def to_hash
        Hash[*keys.map {|key| [key.to_s, get(key)] }.flatten(1)]
      end

      def <=>(other)
        to_hash <=> other
      end

      def each(*args, &block)
        to_hash.each(*args, &block)
      end

      def delegate
        to_hash
      end

      private
      
      def reflect_from(obj) # :nodoc:
        if !obj.nil? and !obj.v8? and !obj.is_a?(Hash)
          obj.class.declared_methods.each { |meth|
            if func_name = meth.to_sym.to_js_func_name
              func = set(func_name, obj.method(meth))
              func.bind(obj)
            end
          }
        end
        self
      end
    end # Object
  end # V8
end # Mustang
