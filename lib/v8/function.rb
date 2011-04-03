module V8
  class Function
    def to_proc
      origin.to_proc if origin
    end
  end # Function
end # V8
