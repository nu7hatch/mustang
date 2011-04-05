# Delegates all missing methods to object specified by <tt>#delegate</tt> method.
module Delegated
  alias_method :old_method_missing, :method_missing
  alias_method :old_respond_to?, :respond_to?

  def method_missing(meth, *args, &block) # :nodoc:
    if (old_respond_to?(:delegate) && delegate.respond_to?(meth))
      delegate.send(meth, *args, &block)
    else
      old_method_missing(meth, *args, &block)
    end
  end

  def respond_to?(meth) # :nodoc:
    if (old_respond_to?(:delegate))
      delegate.respond_to?(meth) or old_respond_to?(meth)
    else
      old_respond_to?(meth)
    end
  end

  def to_s # :nodoc:
    delegate.to_s
  end
end # Delegated
