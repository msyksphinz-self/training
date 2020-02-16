#[derive(PartialEq)]
pub enum OperandType {
    TypeXReg,
    TypeFreg,
    TypeDreg,
    TypeVreg,
    TypeSign,
}

// impl PartialEq for OperandType {
//     fn eq(&self, other: &Self) -> bool {
//         self == other
//     }
// }
// impl Eq for OperandType {}

fn main() {
    println!("Test {:?}", OperandType::TypeXReg == OperandType::TypeXReg);
}
