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

public final class MTF extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "MTF",
          MTF.class,
          null
          );
      addArgument("rt", R.INFO);
      addArgument("fs", F.INFO);}

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final R rt = (R) getArgument("rt", args);
      final F fs = (F) getArgument("fs", args);

      return new MTF(rt, fs);
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final R rt;
  public final F fs;

  public MTF(final R rt, final F fs) {
    assert R.INFO.isSupported(rt);
    assert F.INFO.isSupported(fs);

    this.rt = rt;
    this.fs = fs;

    addArgument("rt", rt);
    addArgument("fs", fs);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("MTF %s, %s", rt.text(vars__), fs.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("01000100100%s%s00000000000", rt.image(vars__), fs.image(vars__));
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    fs.access(pe__, vars__).store(rt.access(pe__, vars__).load());
  }
}