require File.dirname(__FILE__) + '/../../spec_helper'

describe Mustang::V8::Null do
  setup_context

  it "is kind of Mustang::V8::NullClass" do
    subject.should be_kind_of(Mustang::V8::NullClass)
  end

  describe "#null?" do
    it "returns true" do
      subject.should be_null
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
end
