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

package ru.ispras.microtesk.model.vliw.op;

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.memory.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.vliw.PE;
import ru.ispras.microtesk.model.vliw.TempVars;
import ru.ispras.microtesk.model.memory.Location;
import ru.ispras.microtesk.model.vliw.mode.*;

import static ru.ispras.microtesk.model.vliw.TypeDefs.*;

public final class BFF extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "BFF",
          BFF.class,
          null
          );
      addArgument("cc", COND_CODE);
      addArgument("offset", BRANCH_OFFSET.INFO);}

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final Immediate cc = (Immediate) getArgument("cc", args);
      final IsaPrimitive offset = (IsaPrimitive) getArgument("offset", args);

      return new BFF(cc, offset);
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final Immediate cc;
  public final IsaPrimitive offset;

  public BFF(final Immediate cc, final IsaPrimitive offset) {
    assert COND_CODE.equals(cc.getType());
    assert BRANCH_OFFSET.INFO.isSupported(offset);

    this.cc = cc;
    this.offset = offset;

    addArgument("cc", cc);
    addArgument("offset", offset);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("BFF %d, %s", cc.access().load().bigIntegerValue(), offset.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("01000101000%s00%s", cc.access().load().toString(), offset.image(vars__));
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    if (pe__.FCCR.access().bitField(cc.access().load(), cc.access().load()).load().equals(Data.valueOf(Type.INT(1), 0x0))) {
      vars__.BRANCH.access().store(Data.valueOf(BIT, 0x1));
      vars__.JMPADDR.access().store(vars__.NIA.access().load().add(Data.signExtend(WORD, offset.access(pe__, vars__).load()).shiftLeft(Data.valueOf(WORD, 0x3))));
    }
  }
}