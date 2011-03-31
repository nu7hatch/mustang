require File.dirname(__FILE__) + '/../spec_helper'

describe Object do
  setup_context

  it "includes V8::Cast module" do
    subject.respond_to?(:to_v8)
  end
end

describe V8::Cast do
  setup_context

  describe "#to_v8" do
    it "converts current object to v8 compilant" do
      [1,2,3].to_v8.should be_kind_of(V8::Array)
      1.to_v8.should be_kind_of(V8::Integer)
      "foo".to_v8.should be_kind_of(V8::String)
      (1.5).to_v8.should be_kind_of(V8::Number)
      {:foo => 1}.to_v8.should be_kind_of(V8::Object)
    end
  end
end

describe "Typecasting" do
  setup_context

  context "from js to ruby" do
    it "converts null properly" do
      null = cxt.eval("null", "<eval>")
      null.should be_kind_of(V8::Null)
      null.should be_null
    end

    it "converts undefined value to nil" do
      undefined = cxt.eval("undefined", "<eval>")
      undefined.should be_kind_of(V8::Undefined)
      undefined.should be_undefined
    end

    it "converts boolean values properly" do
      cxt.eval("true", "<eval>").should be_true
      cxt.eval("false", "<eval>").should be_false
    end

    it "converts strings properly" do
      cxt.eval("'foo'", "<eval>").should == "foo"
    end

    it "converts integers properly" do
      cxt.eval("1", "<eval>").should == 1
      cxt.eval("-1", "<eval>").should == -1
    end

    it "converts float numbers properly" do
      cxt.eval("1.4", "<eval>").should == 1.4
    end

    it "converts arrays properly" do
      cxt.eval("[1,2,3]", "<eval>").should == [1,2,3]
    end

    it "converts objects properly" do
      obj = cxt.eval("var o = {foo: 1, bar: 2, func: function() { return 1 }}; o", "<eval>")
      obj[:foo].should == 1
      obj[:bar].should == 2
      obj[:func].should be_kind_of(V8::Function)
    end
  end

  context "from ruby to js" do
    it "converts nil to null" do
      cxt[:foo] = nil
      cxt.eval("foo == null", "<eval>").should be
    end

    it "converts boolean values properly" do
      cxt[:foo] = true
      cxt[:bar] = false
      cxt.eval("foo == true", "<eval>").should be
      cxt.eval("bar == false", "<eval>").should be
    end

    it "converts strings properly" do
      cxt[:foo] = "bar"
      cxt.eval("foo == 'bar'", "<eval>").should be
    end

    it "converts integers properly" do
      cxt[:foo] = 1
      cxt[:bar] = -1
      cxt.eval("foo == 1", "<eval>").should be
      cxt.eval("bar == -1", "<eval>").should be
    end

    it "converts numbers properly" do
      cxt[:foo] = 1.5
      cxt.eval("foo == 1.5", "<eval>").should be
    end

    it "converts arrays properly" do
      cxt[:foo] = [1,2,3] 
      cxt.eval("foo[0] == 1", "<eval>").should be
      cxt.eval("foo[1] == 2", "<eval>").should be
      cxt.eval("foo[2] == 3", "<eval>").should be
    end

    it "converts hashes properly" do
      cxt[:foo] = {:a => 1, :b => 2}
      cxt.eval("foo.a == 1", "<eval>").should be
      cxt.eval("foo.b == 2", "<eval>").should be
    end

    it "converts ranges properly" do
      cxt[:foo] = 1..2
      cxt.eval("foo[0] == 1", "<eval>").should be
      cxt.eval("foo[1] == 2", "<eval>").should be
    end
  end
end
