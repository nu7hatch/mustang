require File.dirname(__FILE__) + '/../spec_helper'

describe Class do
  describe "#to_js_func_name" do
    it "returns name without changes when is compatible with js" do
      :foo.to_js_func_name.should == :foo
    end

    it "changes '=' for 'set_' prefix" do
      :foo=.to_js_func_name.should == :set_foo
    end

    it "changes '?' for 'is_' prefix" do
      :foo?.to_js_func_name.should == :is_foo
    end

    it "changes '!' for '_bang' suffix" do
      :foo!.to_js_func_name.should == :foo_bang
    end

    it "returns nil when method name is unconvertable to javascript" do
      [:+, :-, :%, :&, :*, :~, :=~, :/, :>, :<, :>>, :<<, :'=', :'==', :'===', :'<=>'].each { |meth|
        meth.to_js_func_name.should_not be
      }
    end
  end
end
