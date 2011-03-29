require File.dirname(__FILE__) + '/../spec_helper'

describe V8::Value do
  subject { V8::Value }
  setup_context

  describe ".new" do
    it "creates new v8 value" do
      subject.new(Object.new).should be_kind_of(V8::Value)
    end
  end

  describe "#object?" do
    it "returns true when value is an instance of V8::Object" do
      V8::Object.new.should be_object
    end

    it "returns false when value is not an instance of V8::Object" do
      subject.new(Object.new).should_not be_object
    end
  end

  describe "#integer?" do
    it "returns true when value is an instance of V8::Integer" do
      V8::Integer.new(10).should be_integer
    end

    it "returns false when value is not an instance of V8::Integer" do
      subject.new(Object.new).should_not be_integer
    end
  end

  describe "#number?" do
    it "returns true when value is an instance of V8::Number" do
      V8::Number.new(10).should be_number
    end

    it "returns false when value is not an instance of V8::Number" do
      subject.new(Object.new).should_not be_number
    end
  end

  describe "#string?" do
    it "returns true when value is an instance of V8::String" do
      V8::String.new("foo").should be_string
    end

    it "returns false when value is not an instance of V8::String" do
      subject.new(Object.new).should_not be_string
    end
  end

  describe "#external?" do
    it "returns true when value is an instance of V8::External" do
      V8::External.new(Object.new).should be_external
    end

    it "returns false when value is not an instance of V8::External" do
      V8::Integer.new(10).should_not be_external
    end
  end

  describe "#array?" do
    it "returns true when value is an instance of V8::Array" do
      V8::Array.new(1,2,3).should be_array
    end

    it "returns false when value is not an instance of V8::Array" do
      subject.new(Object.new).should_not be_array
    end
  end

  describe "#function?" do
    it "returns true when value is an instance of V8::Integer" do
      pending
    end

    it "returns false when value is not an instance of V8::Integer" do
      subject.new(Object.new).should_not be_integer
    end
  end

  describe "#to_integer" do
    it "returns V8 integer representation of value" do
      val = V8::Integer.new(10)
      subject.new(val).to_integer.should == val
    end
  end

  describe "#to_string" do
    it "returns V8 string representation of value" do
      val = V8::String.new("foo")
      subject.new(val).to_string.should == val
    end
  end

  describe "#to_number" do
    it "returns V8 number representation of value" do
      val = V8::Number.new(10.5)
      subject.new(val).to_number.should == val
    end
  end

  describe "#to_object" do
    it "returns V8 object representation of value" do
      val = V8::Object.new(:foo => 1)
      subject.new(val).to_object.should == val
    end
  end

  describe "#===" do
    it "returns true when compared objects are strictly the same" do
      obj = V8::Object.new(:foo => 1)
      val = subject.new(obj)
      val.should === obj
    end

    it "returns false when compared objects are strictly different" do
      obj = V8::Object.new(:foo => 1)
      val = subject.new(V8::Array.new)
      val.should_not === obj
    end
  end

  describe "#==" do
    it "returns true when compared objects are the same" do
      obj1 = V8::Integer.new(1)
      obj2 = V8::Integer.new(1)
      subject.new(obj1).should == obj2
    end

    it "returns false when compared objects are different" do
      obj1 = V8::Object.new(:foo => 1)
      obj2 = V8::Object.new(:bar => 1)
      subject.new(obj1).should_not == obj2
    end
  end
end
