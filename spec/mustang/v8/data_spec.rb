require File.dirname(__FILE__) + '/../../spec_helper'

describe Mustang::V8::Data do
  setup_context

  describe "#error?" do
    it "returns false" do
      subject.should_not be_error
    end
  end
  
  describe "#null?" do
    it "returns false" do
      subject.should_not be_null
    end
  end

  describe "#undefined?" do
    it "returns false" do
      subject.should_not be_undefined
    end
  end

  describe "#empty?" do
    it "returns false" do
      subject.should_not be_empty
    end
  end

  describe "#value?" do
    it "returns true when data is kind of value" do
      Mustang::V8::Integer.new(1).should be_value
    end

    it "returns false when data is not kind of value" do
      Mustang::V8::Undefined.should_not be_value
    end
  end
end
