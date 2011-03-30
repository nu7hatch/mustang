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
 
    describe "info methods" do
      before do
        cxt.eval("var foo=1;\nbroken$code [{", "<eval>")
      end

      describe "#message" do
        it "returns error message string" do
          cxt.errors.last.message.should == "Unexpected end of input"
        end
      end
      
      describe "#line_no" do
        it "returns broken line number" do
          cxt.errors.last.line_no.should == 2
        end
      end

      describe "#script_name" do
        it "returns broken script name" do
          cxt.errors.last.script_name.should == "<eval>"
        end
      end

      describe "#source_line" do
        it "returns broken source line" do
          cxt.errors.last.source_line.should == "broken$code [{"
        end
      end

      describe "#column_start_no" do
        it "returns start column of broken code" do
          cxt.errors.last.column_start_no.should == 14
        end
      end

      describe "#column_end_no" do
        it "returns end column of broken code" do
          cxt.errors.last.column_end_no.should == 14
        end
      end
    end
  end
end
