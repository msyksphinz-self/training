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

package ru.ispras.microtesk.model.x86.op;

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.memory.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.x86.PE;
import ru.ispras.microtesk.model.x86.TempVars;
import ru.ispras.microtesk.model.memory.Location;

import static ru.ispras.microtesk.model.x86.TypeDefs.*;

public final class inc_proc_word extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "inc_proc_word",
          inc_proc_word.class,
          null
          );
      addArgument("op1", WORD);}

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final Immediate op1 = (Immediate) getArgument("op1", args);

      return new inc_proc_word(op1);
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final Immediate op1;

  public inc_proc_word(final Immediate op1) {
    assert WORD.equals(op1.getType());

    this.op1 = op1;

    addArgument("op1", op1);
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    vars__.tmpw_res.access().store(Data.zeroExtend(Type.CARD(17), op1.access().load().add(Data.valueOf(WORD, 0x1))));
    pe__.OF.access().store(Data.valueOf(BIT, op1.access().bitField(15, 15).load().equals(Data.valueOf(Type.CARD(1), 0x0)) && vars__.tmpw_res.access().bitField(15, 15).load().equals(Data.valueOf(Type.CARD(1), 0x1))));
    pe__.ZF.access().store(Data.valueOf(BIT, vars__.tmpw_res.access().bitField(15, 0).load().equals(Data.valueOf(Type.CARD(16), 0x0))));
    pe__.SF.access().store(Data.valueOf(BIT, vars__.tmpw_res.access().bitField(15, 15).load().equals(Data.valueOf(Type.CARD(1), 0x1))));
  }
}