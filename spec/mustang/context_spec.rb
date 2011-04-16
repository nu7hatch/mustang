require File.dirname(__FILE__) + '/../spec_helper'

describe Mustang::Context do
  it "inherits Mustang::V8::Context" do
    subject.should be_kind_of(Mustang::V8::Context)
  end

  describe ".new" do
    context "when locals given" do
      it "sets it in global prototype" do
        cxt = Mustang::Context.new(:foo => 1, :bar => 2)
        cxt[:foo].should == 1
        cxt[:bar].should == 2
      end
    end
  end

  describe "#evaluate" do
    context "when locals given" do
      it "sets it before evaluation" do
        subject.evaluate("foo == 120", :foo => 120).should be_true
      end
    end

    context "by default" do
      it "sets <eval> filename" do
        # passed...
      end
    end
  end

  describe "#load" do
    context "when existing files specified" do
      it "loads and executes given files" do
        res1 = subject.load(File.expand_path("../../fixtures/test1.js", __FILE__))
        res1.should == 'test1foo'
        res2 = subject.load(File.expand_path("../../fixtures/test2.js", __FILE__))
        res2.should == 'test2bar'
      end
    end

    context "when file not found" do
      it "raises ScriptNotFoundError" do
        expect { subject.load("notexists.js") }.to raise_error(Errno::ENOENT, "No such file or directory - script file `notexists.js' does not exist.")
      end
    end
  end
end
