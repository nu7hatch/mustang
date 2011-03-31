require 'support/delegated'

module V8
  class Date
    include Comparable
    include Delegated

    def to_time
      Time.at(to_i);
    end
    
    def <=>(other)
      to_time <=> other
    end

    def delegate
      to_time
    end
  end # Date
end # V8
