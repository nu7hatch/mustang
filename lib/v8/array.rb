require 'support/delegated'

module V8
  class Array
    include Comparable
    include Enumerable
    include Delegated

    def <=>(other)
      to_a <=> other
    end

    def each(*args, &block)
      to_a.each(*args, &block)
    end

    def delegate
      to_a
    end
  end # Array
end # V8
