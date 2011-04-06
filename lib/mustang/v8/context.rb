module Mustang
  module V8
    class Context
      def get(key)
        prototype[key]
      end
      alias_method :[], :get

      def set(key, value)
        prototype[key] = value
      end
      alias_method :[]=, :set
    end # Context
  end # V8
end # Mustang
