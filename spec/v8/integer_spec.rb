require File.dirname(__FILE__) + '/../spec_helper'

describe V8::Integer do 
  subject { V8::Integer }
  setup_context
  
  it "inherits V8::Object" do
    subject.new(10).should be_kind_of(V8::Object)
  end

  describe ".new" do
    it "creates new v8 integer based on passed value" do
      subject.new(10).to_i.should == 10
      subject.new(-10).to_i.should == -10
    end
  end

  describe "#to_i" do
    it "returns fixnum value of represented integer" do
      subject.new(10).to_i.should == 10
    end
  end

  describe "#to_s" do
    it "returns string representation of referenced value" do
      subject.new(10).to_s.should == "10"
    end
  end

  describe "#to_f" do
    it "returns float representation of referenced value" do
      subject.new(10).to_f.should == 10.0
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
  end
end
