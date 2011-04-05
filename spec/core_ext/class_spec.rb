require File.dirname(__FILE__) + '/../spec_helper'

describe Class do
  describe "#declared_methods" do
    it "returns all defined and inherited methods but not defined in the Object" do
      class First
        attr_accessor :foo
        def to_s; end
      end

      class Second < First
        attr_reader :bar
      end

      First.declared_methods.should == [:foo, :foo=, :to_s]
      Second.declared_methods.should == [:bar, :foo, :foo=, :to_s]
    end
  end
end
