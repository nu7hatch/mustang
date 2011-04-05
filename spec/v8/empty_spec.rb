require File.dirname(__FILE__) + '/../spec_helper'

describe V8::Empty do
  setup_context

  it "is kind of V8::EmptyClass" do
    subject.should be_kind_of(V8::EmptyClass)
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
    it "returns true" do
      subject.should be_empty
    end
  end
end
