require File.dirname(__FILE__) + '/../../spec_helper'

describe Mustang::V8::Undefined do
  setup_context

  it "is kind of Mustang::V8::UndefinedClass" do
    subject.should be_kind_of(Mustang::V8::UndefinedClass)
  end

  describe "#null?" do
    it "returns false" do
      subject.should_not be_null
    end
  end

  describe "#undefined?" do
    it "returns true" do
      subject.should be_undefined
    end
  end

  describe "#empty?" do
    it "returns false" do
      subject.should_not be_empty
    end
  end
end
