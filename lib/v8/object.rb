module V8
  class Object
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
