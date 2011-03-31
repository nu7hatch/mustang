require File.dirname(__FILE__) + '/../spec_helper'

describe V8::Primitive do 
  setup_context
  
  it "inherits V8::Value" do
    V8::Primitive.new(0).should be_kind_of(V8::Value)
  end
end
