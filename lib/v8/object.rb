module V8
  class Object
    class << self
      alias_method :native_new, :new

      def new(opts={})
        obj = native_new
        opts.each { |key, value| obj.set(key, value) }
        obj
      end
    end
    
    include Comparable
    include Enumerable

    def <=>(other)
      to_hash <=> other
    end

    def each(*args, &block)
      to_hash.each(*args, &block)
    end

    def to_hash
      Hash[*keys.map {|key| [key, get(key)] }.flatten(1)]
    end

    def to_s
      to_hash.to_s
    end
  end # Integer
end # V8
