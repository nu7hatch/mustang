require 'support/delegated'

module V8
  class Object
    include Comparable
    include Enumerable
    #include Delegated

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
  end # Integer
end # V8
