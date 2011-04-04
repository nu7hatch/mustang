require 'support/delegated'

module V8
  class Object
    class << self
      alias_method :native_new, :new

      def new(*args)
        orig = args.first
        obj = native_new(*args)

        unless orig.kind_of?(Hash)
          (orig.public_methods - Object.public_methods).each { |meth|
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

    #def respond_to?(meth)
    # !self[meth].nil?
    #end

    #def method_missing(meth, *args, &block)
    #end

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
