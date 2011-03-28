require File.dirname(__FILE__) + '/../spec_helper'

describe V8::Integer do 
  subject { V8::Integer }
  setup_context
  
  it "inherits V8::Object" do
    subject.new(10).should be_kind_of(V8::Object)
  end

  describe ".new" do
    it "creates new v8 integer based on passed value" do
      subject.new(10).should == 10
      subject.new(-10).should == -10
    end
  end

  describe "#to_i" do
    it "returns fixnum value of represented integer" do
      subject.new(10).to_i.should == 10
    end
  end

  describe "an instance" do
    it "is comparable" do
      int = subject.new(10)
      int.should == 10
      int.should_not == 20
      int.should > 5
      int.should < 15
      int.should <= 10
      int.should >= 10
    end
    
    it "is delegated properly" do
      int = subject.new(3)
      int.delegate.should == int.to_i
    end
  end
end
