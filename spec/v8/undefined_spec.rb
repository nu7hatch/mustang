require File.dirname(__FILE__) + '/../spec_helper'

describe V8::Undefined do
  setup_context

  it "includes Singleton" do
    subject.class.included_modules.should include(Singleton)
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
