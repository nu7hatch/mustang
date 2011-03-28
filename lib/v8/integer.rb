require 'support/delegated'

module V8
  class Integer
    include Comparable
    include Delegated

    def <=>(other)
      to_i <=> other
    end

    def delegate
      to_i
    end
  end # Integer
end # V8
