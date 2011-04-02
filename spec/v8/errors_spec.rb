require File.dirname(__FILE__) + '/../spec_helper'

describe "Exception handling" do
  setup_context

  it "returns v8 error object when JS code is broken" do
    exc = cxt.eval("broken$code", "<eval>")
    exc.should be_kind_of(V8::Error)
    exc.should be_error
  end
end

describe V8::Error do
  setup_context

  it "inherits V8::Data" do
    subject.should be_kind_of(V8::Data)
  end

  it "#reference_error? returns false" do
    subject.should_not be_reference_error
  end
  
  it "#syntax_error? returns false" do
    subject.should_not be_syntax_error
  end
  
  it "#range_error? returns false" do
    subject.should_not be_range_error
  end
  
  it "#error? return true" do
    subject.should be_error
  end

  describe "info methods" do
    let(:exc) do
      cxt.eval("var a=1;\nbroken$code [{", "<eval>")
    end

    describe "#message" do
      it "returns error message string" do
        exc.message.should == "Unexpected end of input"
      end
    end
    
    describe "#line_no" do
      it "returns broken line number" do
        exc.line_no.should == 2
      end
    end

    describe "#script_name" do
      it "returns broken script name" do
        exc.script_name.should == "<eval>"
      end
    end

    describe "#source_line" do
      it "returns broken source line" do
        exc.source_line.should == "broken$code [{"
      end
    end

    describe "#start_col" do
      it "returns start column of broken code" do
        exc.start_col.should == 14
      end
    end

    describe "#end_col" do
      it "returns end column of broken code" do
        exc.end_col.should == 14
      end
    end
  end
end

describe "Returned error" do
  setup_context

  it "is an ReferenceError when using undefined reference" do
    cxt.eval("broken$code", "<eval>").should be_reference_error 
  end

  it "is an SyntaxError when invalid syntax" do
    cxt.eval("broken {[/", "<eval>").should be_syntax_error
  end

  it "is an RangeError when maximum call stack exceeded" do
    cxt.eval("function a() { a() }; a()", "<eval>").should be_range_error
  end
end

describe V8::ReferenceError do
  it "#reference_error? returns true" do
    subject.should be_reference_error
  end

  it "#error? returns true" do
    subject.should be_error
  end
end

describe V8::SyntaxError do
  it "#syntax_error? returns true" do
    subject.should be_syntax_error
  end

  it "#error? returns true" do
    subject.should be_error
  end
end

describe V8::RangeError do
  it "#range_error? returns true" do
    subject.should be_range_error
  end

  it "#error? returns true" do
    subject.should be_error
  end
end

