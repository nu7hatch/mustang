require File.dirname(__FILE__) + '/../../spec_helper'

describe Mustang::V8::Integer do 
  subject { Mustang::V8::Integer }
  setup_context
  
  it "inherits Mustang::V8::Number" do
    pending "Needs to separate Mustang::V8::Number"
    subject.new(10).should be_kind_of(Mustang::V8::Number)
  end

  it "is kind of Fixnum" do
    subject.new(1).should be_kind_of Fixnum
  end

  describe ".new" do
    context "when no context entered" do
      it "should raise error" do
        Mustang::V8::Context.exit_all!
        expect { subject.new(1) }.to raise_error(RuntimeError, "can't create V8 object without entering into context")
      end
    end

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
