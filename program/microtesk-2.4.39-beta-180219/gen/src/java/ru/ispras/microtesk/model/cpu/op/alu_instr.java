/*
 * Copyright (c) Institute for System Programming of the Russian Academy of Sciences
 * All Rights Reserved
 *
 * Institute for System Programming of the Russian Academy of Sciences (ISP RAS)
 * 25 Alexander Solzhenitsyn st., Moscow, 109004, Russia
 * http://www.ispras.ru
 *
 * The present file was automatically generated on the basis of formal specifications.
 * It is distributed under the same terms and conditions as the derived specifications.
 *
 * N.B. PLEASE DO NOT MODIFY THIS FILE.
 */

package ru.ispras.microtesk.model.cpu.op;

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.memory.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.cpu.PE;
import ru.ispras.microtesk.model.cpu.TempVars;
import ru.ispras.microtesk.model.cpu.mode.*;

import static ru.ispras.microtesk.model.cpu.TypeDefs.*;

public final class alu_instr extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "alu_instr",
          alu_instr.class,
          null
          );
      addArgument("command", ALU.INFO);
      addArgument("op1", OPRNDL.INFO);
      addArgument("op2", OPRNDR.INFO);}

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final IsaPrimitive command = (IsaPrimitive) getArgument("command", args);
      final IsaPrimitive op1 = (IsaPrimitive) getArgument("op1", args);
      final IsaPrimitive op2 = (IsaPrimitive) getArgument("op2", args);

      return new alu_instr(command, op1, op2);
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final IsaPrimitive command;
  public final IsaPrimitive op1;
  public final IsaPrimitive op2;

  public alu_instr(final IsaPrimitive command, final IsaPrimitive op1, final IsaPrimitive op2) {
    assert ALU.INFO.isSupported(command);
    assert OPRNDL.INFO.isSupported(op1);
    assert OPRNDR.INFO.isSupported(op2);

    this.command = command;
    this.op1 = op1;
    this.op2 = op2;

    addArgument("command", command);
    addArgument("op1", op1);
    addArgument("op2", op2);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("%s %s %s", command.text(vars__), op1.text(vars__), op2.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("%s%s00%s", command.image(vars__), op1.image(vars__), op2.image(vars__));
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    vars__.SRC1.access().store(op1.access(pe__, vars__).load());
    vars__.SRC2.access().store(op2.access(pe__, vars__).load());
    command.execute(pe__, vars__);
    op1.access(pe__, vars__).store(vars__.DEST.access().load());
    pe__.PC.access().store(pe__.PC.access().load().add(Data.valueOf(INDEX, 0x2)));
  }
}