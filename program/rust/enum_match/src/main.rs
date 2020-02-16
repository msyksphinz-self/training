enum OperandType {
    TypeXReg,
    TypeFreg,
    TypeDreg,
    TypeVreg,
    TypeSign,
}


fn match_type(x: OperandType) {
    match x {
        OperandType::TypeXReg => { println!("TypeXReg"); }
        OperandType::TypeFreg => { println!("TypeFreg"); }
        OperandType::TypeDreg => { println!("TypeDreg"); }
        OperandType::TypeVreg => { println!("TypeVreg"); }
        OperandType::TypeSign => { println!("TypeSign"); }
    };
}

fn main() {

    match_type(OperandType::TypeXReg);
    match_type(OperandType::TypeFreg);
    match_type(OperandType::TypeDreg);
    match_type(OperandType::TypeVreg);
    match_type(OperandType::TypeSign);

}
