require File.dirname(__FILE__) + '/../spec_helper'

describe "Exception handling" do
  setup_context

  before do 
    cxt.eval("broken$code", "<eval>")
  end

  it "catches and stores js exceptions" do
    cxt.errors.should have(1).items
  end

  describe V8::Exception do 
    describe "#message" do
      it "contains error message string" do
        cxt.errors.first.message.should == "foo"
      end
    end
  end
end
