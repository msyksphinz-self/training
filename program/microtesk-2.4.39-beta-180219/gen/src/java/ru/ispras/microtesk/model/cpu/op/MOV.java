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

public final class MOV extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "MOV",
          MOV.class,
          null
          );
      addShortcut(new Info_alu_instr(), "#root");
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      return new MOV();
    }
  }
  private static final class Info_alu_instr extends IsaPrimitiveInfoAnd {
    Info_alu_instr() {
      super(
          IsaPrimitiveKind.OP,
          "MOV",
          alu_instr.class,
          null
          );
      addArgument("op1", OPRNDL.INFO);
      addArgument("op2", OPRNDR.INFO);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final IsaPrimitive op1 = (IsaPrimitive) getArgument("op1", args);
      final IsaPrimitive op2 = (IsaPrimitive) getArgument("op2", args);

      return new alu_instr(
        new MOV(),
        op1,
        op2
      );
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public MOV() {
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("mov");
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("10");
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    vars__.DEST.access().store(vars__.SRC2.access().load());
  }
}