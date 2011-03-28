module V8
  class Number
    include Comparable

    def <=>(other)
      to_f <=> other
    end

    def to_s
      to_f.to_s
    end

    def to_i
      to_f.to_i
    end
  end # Number
end # V8
