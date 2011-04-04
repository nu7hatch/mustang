require 'support/delegated'

module V8
  class Object
    class << self
      alias_method :native_new, :new

      def new(*args)
        orig = args.first
        obj = native_new(*args)

        if !orig.nil? and !orig.v8? and !orig.is_a?(Hash)
          orig.class.instance_methods(false).each { |meth|
            jsmeth = to_js_method_name(meth)
            obj[jsmeth] = orig.method(meth)
            obj[jsmeth].bind(orig)
          }
        end

        obj
      end

      private

      def to_js_method_name(name)
        jsname = name.to_s.dup
        jsname = jsname.gsub(/\!$/, "") if jsname.include?("!")
        jsname = "is_#{jsname.gsub(/\?$/, "")}" if jsname.include?("?")
        jsname = "set_#{jsname.gsub(/\=$/, "")}" if jsname.include?("=")
        jsname
      end
    end

    def respond_to?(meth)
      !self[meth].nil?
    end

    def method_missing(meth, *args, &block)
      if respond_to?(meth)
        property = get(meth)

        if property.v8? and property.value? and property.function?
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
  end # Object
end # V8
