require File.dirname(__FILE__) + '/../../spec_helper'

describe Mustang::V8::Value do
  subject { Mustang::V8::Value }
  setup_context

  it "inherits Mustang::V8::Data" do
    subject.new(Object.new).should be_kind_of(Mustang::V8::Data)
  end
  
  describe ".new" do
    context "when no context entered" do
      it "should raise error" do
        Mustang::V8::Context.exit_all!
        expect { subject.new(Object.new) }.to raise_error(RuntimeError, "can't create V8 object without entering into context")
      end
    end

    it "creates new v8 value" do
      subject.new(Object.new).should be_kind_of(Mustang::V8::Value)
    end
  end

  describe "#object?" do
    it "returns true when value is an instance of Mustang::V8::Object" do
      Mustang::V8::Object.new.should be_object
    end

    it "returns false when value is not an instance of Mustang::V8::Object" do
      Mustang::V8::External.new(Object.new).should_not be_object
    end

    it "is aliased with #obj?" do
      Mustang::V8::Object.new.should be_obj
    end
  end

  describe "#integer?" do
    it "returns true when value is an instance of Mustang::V8::Integer" do
      Mustang::V8::Integer.new(10).should be_integer
    end

    it "returns false when value is not an instance of Mustang::V8::Integer" do
      subject.new(Object.new).should_not be_integer
    end

    it "is aliased with #int?" do
      Mustang::V8::Integer.new(10).should be_int
    end
  end

  describe "#number?" do
    it "returns true when value is an instance of Mustang::V8::Number" do
      Mustang::V8::Number.new(10).should be_number
    end

    it "returns false when value is not an instance of Mustang::V8::Number" do
      subject.new(Object.new).should_not be_number
    end

    it "is aliased with #num?" do
      Mustang::V8::Number.new(10).should be_num
    end
  end

  describe "#string?" do
    it "returns true when value is an instance of Mustang::V8::String" do
      Mustang::V8::String.new("foo").should be_string
    end

    it "returns false when value is not an instance of Mustang::V8::String" do
      subject.new(Object.new).should_not be_string
    end

    it "is aliased with #str?" do
      Mustang::V8::String.new("foo").should be_str
    end
  end

  describe "#external?" do
    it "returns true when value is an instance of Mustang::V8::External" do
      Mustang::V8::External.new(Object.new).should be_external
    end

    it "returns false when value is not an instance of Mustang::V8::External" do
      Mustang::V8::Integer.new(10).should_not be_external
    end
  end

  describe "#array?" do
    it "returns true when value is an instance of Mustang::V8::Array" do
      Mustang::V8::Array.new(1,2,3).should be_array
    end

    it "returns false when value is not an instance of Mustang::V8::Array" do
      subject.new(Object.new).should_not be_array
    end

    it "is aliased with #ary?" do
      Mustang::V8::Array.new.should be_ary
    end
  end

  describe "#function?" do
    it "returns true when value is an instance of Mustang::V8::Function" do
      Mustang::V8::Function.new(proc {}).should be_function
    end

    it "returns false when value is not an instance of Mustang::V8::Function" do
      Mustang::V8::Integer.new(1).should_not be_function
    end

    it "is aliased with #func?" do
      Mustang::V8::Function.new(proc {}).should be_func
    end
  end

  describe "#regexp?" do
    it "returns true when value is an instance of Mustang::V8::Regexp" do
      Mustang::V8::Regexp.new(/foo/).should be_regexp
    end

    it "returns false when value is not an instance of Mustang::V8::Regexp" do
      Mustang::V8::Integer.new(1).should_not be_regexp
    end

    it "is aliased with #regex?" do
      Mustang::V8::Regexp.new(/foo/).should be_regex
    end
  end

  describe "#date?" do
    it "returns true when value is an instance of Mustang::V8::Date" do
      Mustang::V8::Date.new(Time.now).should be_date
    end

    it "returns false when value is not an instance of Mustang::V8::Date" do
      Mustang::V8::Integer.new(1).should_not be_date
    end
  end

  describe "#to_integer" do
    it "returns V8 integer representation of value" do
      val = Mustang::V8::Integer.new(10)
      subject.new(val).to_integer.should == val
    end
  end

  describe "#to_string" do
    it "returns V8 string representation of value" do
      val = Mustang::V8::String.new("foo")
      subject.new(val).to_string.should == val
    end
  end

  describe "#to_number" do
    it "returns V8 number representation of value" do
      val = Mustang::V8::Number.new(10.5)
      subject.new(val).to_number.should == val
    end
  end

  describe "#to_object" do
    it "returns V8 object representation of value" do
      val = Mustang::V8::Object.new(:foo => 1)
      subject.new(val).to_object.should == val
    end
  end

  describe "#to_boolean" do
    it "return V8 boolean representation of value" do
      Mustang::V8::Object.new(:foo => 1).to_boolean.should == true
      Mustang::V8::Integer.new(0).to_boolean.should == false
    end
  end

  describe "#===" do
    it "returns true when compared objects are strictly the same" do
      obj = Mustang::V8::Object.new(:foo => 1)
      val = subject.new(obj)
      val.should === obj
    end

    it "returns false when compared objects are strictly different" do
      obj = Mustang::V8::Object.new(:foo => 1)
      val = subject.new(Mustang::V8::Array.new)
      val.should_not === obj
    end
  end

  describe "#==" do
    it "returns true when compared objects are the same" do
      obj1 = Mustang::V8::Integer.new(1)
      obj2 = Mustang::V8::Integer.new(1)
      subject.new(obj1).should == obj2
    end

    it "returns false when compared objects are different" do
      obj1 = Mustang::V8::Object.new(:foo => 1)
      obj2 = Mustang::V8::Object.new(:bar => 1)
      subject.new(obj1).should_not == obj2
    end
  end

  describe "null?" do
    it "returns true when object is not null" do
      Mustang::V8::Object.new.should_not be_null
    end
  end

  describe "undefined?" do
    it "returns true when object is not undefined" do
      Mustang::V8::Object.new.should_not be_undefined
    end
  end

  describe "empty?" do
    it "returns true when object is not empty" do
      Mustang::V8::Object.new.should_not be_empty
    end
  end
end
