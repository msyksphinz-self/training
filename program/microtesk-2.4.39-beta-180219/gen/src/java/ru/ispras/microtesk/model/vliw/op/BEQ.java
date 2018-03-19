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
import ru.ispras.microtesk.model.vliw.mode.*;

import static ru.ispras.microtesk.model.vliw.TypeDefs.*;

public final class BEQ extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "BEQ",
          BEQ.class,
          null
          );
      addArgument("rs", R.INFO);
      addArgument("rt", R.INFO);
      addArgument("offset", BRANCH_OFFSET.INFO);}

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final R rs = (R) getArgument("rs", args);
      final R rt = (R) getArgument("rt", args);
      final IsaPrimitive offset = (IsaPrimitive) getArgument("offset", args);

      return new BEQ(rs, rt, offset);
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final R rs;
  public final R rt;
  public final IsaPrimitive offset;

  public BEQ(final R rs, final R rt, final IsaPrimitive offset) {
    assert R.INFO.isSupported(rs);
    assert R.INFO.isSupported(rt);
    assert BRANCH_OFFSET.INFO.isSupported(offset);

    this.rs = rs;
    this.rt = rt;
    this.offset = offset;

    addArgument("rs", rs);
    addArgument("rt", rt);
    addArgument("offset", offset);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("BEQ %s, %s, %s", rs.text(vars__), rt.text(vars__), offset.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("000010%s%s%s", rs.image(vars__), rt.image(vars__), offset.image(vars__));
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    if (rs.access(pe__, vars__).load().equals(rt.access(pe__, vars__).load())) {
      vars__.BRANCH.access().store(Data.valueOf(BIT, 0x1));
      vars__.JMPADDR.access().store(vars__.NIA.access().load().add(Data.signExtend(WORD, offset.access(pe__, vars__).load()).shiftLeft(Data.valueOf(WORD, 0x3))));
    }
  }
}