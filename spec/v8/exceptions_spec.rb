require File.dirname(__FILE__) + '/../spec_helper'

describe "Exception handling" do
  setup_context

  it "catches and stores js exceptions" do
    cxt.eval("broken$code", "<eval>")
    cxt.errors.should have(1).items
  end

  describe V8::Exception do
    describe "types" do
      describe "ReferenceError" do
        before do
          cxt.eval("broken$code", "<eval>")
        end

        it "has proper type" do
          cxt.errors.last.should be_kind_of(V8::ReferenceError);
        end

        it "#reference_error? returns true" do
          cxt.errors.last.should be_reference_error
        end
      end

      describe "SyntaxError" do
        before do
          cxt.eval("broken {[/}", "<eval>")
        end

        it "has proper type" do
          cxt.errors.last.should be_kind_of(V8::SyntaxError);
        end

        it "#syntax_error? returns true" do
          cxt.errors.last.should be_syntax_error
        end
      end

      describe "RangeError" do
        before do
          cxt.eval("function a() { a() }; a()", "<eval>")
        end
        
        it "has proper type" do
          cxt.errors.last.should be_kind_of(V8::RangeError);
        end

        it "#range_error? returns true" do
          cxt.errors.last.should be_range_error
        end
      end
    end
 
    describe "#message" do
      it "contains error message string" do
        cxt.eval("broken$code [{", "<eval>")
        debug
        cxt.errors.last.message.should == "foo"
      end
    end
  end
end
