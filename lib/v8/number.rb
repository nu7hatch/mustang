require 'support/delegated'

module V8
  class Number
    include Comparable
    include Delegated

    def <=>(other)
      to_f <=> other
    end

    def delegate
      to_f
    end
  end # Number
end # V8
