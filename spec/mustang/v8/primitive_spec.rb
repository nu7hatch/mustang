require File.dirname(__FILE__) + '/../../spec_helper'

describe Mustang::V8::Primitive do 
  setup_context
  
  it "inherits Mustang::V8::Value" do
    Mustang::V8::Primitive.new(0).should be_kind_of(Mustang::V8::Value)
  end
end
