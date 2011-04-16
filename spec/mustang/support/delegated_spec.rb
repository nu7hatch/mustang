require File.dirname(__FILE__) + '/../../spec_helper'

class MockDelegate
  attr_reader :n

  def initialize(n)
    @n = n
  end

  def foo
    "bar"
  end
end

class MockDelegated
  include Delegated

  def delegate
    @d = MockDelegate.new(10)
  end
end

describe Delegated do
  subject { MockDelegated.new }

  describe "#respond_to?" do
    it "returns true when delegate has given method" do
      subject.should respond_to(:foo)
    end

    it "returns false when delegate don't have given method" do
      subject.should_not respond_to(:bar)
    end
  end

  describe "#method_missing" do
    it "calls delegate method if exists" do
      subject.foo.should == 'bar'
      subject.n.should == 10
    end

    it "raises NoMethodError otherwise" do
      expect { subject.bar }.to raise_error(NoMethodError)
    end
  end
end
