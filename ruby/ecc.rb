require 'openssl'
require 'pp'

pp OpenSSL::PKey::EC.builtin_curves
ec = OpenSSL::PKey::EC.new('secp384r1')
ec.generate_key
p ec.private_key.methods - Object.methods
p ec.private_key.num_bits
pub = OpenSSL::PKey::EC.new(ec.group.curve_name)
pub.public_key = ec.public_key
p pub.public_key.methods - Object.methods
p pub.public_key.to_bn.methods - Object.methods
p ec.group.curve_name
