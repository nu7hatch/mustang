require File.dirname(__FILE__) + '/../../spec_helper'

describe Object do
  setup_context

  it "includes V8::Cast module" do
    Object.included_modules.should include(Mustang::V8::Cast)
  end

  describe "#v8?" do
    it "returns false when object is not reflection from v8" do
      Object.new.should_not be_v8
    end
    
    it "returns true when object is reflection from v8" do
      Mustang::V8::Integer.new(1).should be_v8
    end
  end
end
